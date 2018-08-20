import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class RMIServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6013);

            // now listen for connections
            while (true) {
                Socket client = sock.accept();
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

                // write the Data to the socket
                pout.println(new Date().toString());

                //close the socket an dresume
                //listening for connection
                client.close();
            }
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}