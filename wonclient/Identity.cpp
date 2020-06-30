#include "pch.h"

#include "Identity.h"


namespace MINTCLIENT
{
    Win32::Thread identityThread;
    U32 STDCALL Process(void* context)
    {
        auto* command_list = static_cast<Client::CommandList*>(context);

        command_list->PassToClient();

        bool done_processing = false;

        while (!done_processing)
        {
            if (command_list->Busy()) {
                void* ctx;
                if (command_list->GetEvents()->Wait(ctx, FALSE, MINTCLIENT::DefaultTimeout))
                {
                    auto* cmd = static_cast<Client::MINTCommand*>(ctx);

                    switch (cmd->command_id)
                    {
                        case MINTCLIENT::Message::IdentityAuthenticate: // 0xCD5AF72B
                        {
                            Identity::Result result;
                            result.error = cmd->GetError();
                            result.context = cmd->context;
                            cmd->callback(result);
                            cmd->DropFromClient();
                        }
                        break;
                    }
                }
                else
                {
                    command_list->TimeoutAll();
                }
            }
            else
            {
                done_processing = true;
            }
        }

        command_list->ShutdownClients();
        delete command_list;
        return TRUE;
    }


    //
    // MINT::Identity::Authenticate
    //
    WONAPI::Error Identity::Authenticate(MINTCLIENT::Client* client, const CH* username, const CH* password, void (*callback)(const Identity::Result& result), void* context)
    {
        auto* command_list = new MINTCLIENT::Client::CommandList();

        // Create a `MINTCommand` with the command and data to send to the server.
        auto* cmd = new Client::MINTCommand(client);
        cmd->command_id = MINTCLIENT::Message::IdentityAuthenticate;
        cmd->callback = callback;

        auto identity = Data::Identity();
        identity.username.Set(username);
        identity.password.Set(password);
        cmd->SetDataFromStruct(identity);
        cmd->SetContext(context);

        command_list->Add(cmd);

        if (!identityThread.IsAlive()) {
            identityThread.Start(Process, command_list);
        }

        return WONAPI::Error_Pending;
    }
}
