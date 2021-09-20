package server;

import java.net.SocketException;
import java.net.UnknownHostException;
/**
 * @author Christoffer Rova
 * @version 1.0
 * date 2020-10-09
 * The class needed to start the server and its GUI.
 */
public class main {

	public static void main(String[] args) throws SocketException, UnknownHostException {
		// Create a server object and give port number as a parameter.
		Server server = new Server(4444);
	}

}
