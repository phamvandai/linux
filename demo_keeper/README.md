# Demo application

Initapp initializes resource, then launch keeper.
Keeper will launch mainapp and monitor it, if mainapp crashed, it will be launched again.
Mainapp can be lauched from keeper only.

Mainapp is a openssl server, client is openssl client. Client will connect & send data to server,
then receives response.

# Server
Build: ./demo_server.sh build
Clean: ./demo_server.sh clean
Run:   ./demo_server.sh run

# Client
Build: ./demo_client.sh build
Clean: ./demo_client.sh clean
Run:   ./demo_client.sh run

# Some notes 
- If parent process died before child died, init will be new parent -> getchar, scanf failed.
Once the parent process finishes, control is returned to shell; and stdin could be closed.
To retain child's access to stdin, you can let the parent wait until the child is done.
- SIGKILL can not be caught. We can normally exit all apps if send other signals which can be
caught by keeper.
- Child process in background can not block on getchar()
