import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 * @author: haoliu on 20/08/2018 19:15
 */
public class RMIClient {
    public static void main(String[] args) {
        try {
            //make connection to server socket
            Socket socket = new Socket("127.0.0.1", 6013);

            InputStream is = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));

            //read the data from the socket
            String line;
            while ((line = reader.readLine()) != null)
                System.out.println(line);

            //close the socket connection
            socket.close();
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
