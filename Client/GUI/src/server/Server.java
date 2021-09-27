package server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Observable;

/**
 * @author Christoffer Rova
 * @version 1.0
 * date 2020-10-09
 * This class is needed for handling of GUI and communication between server and client.
 */
public class Server extends Observable {

	Board board;
	GUI gui;
	DatagramSocket socketUDP;
	packetReader reader;
	Thread thread;
	/**
	 * The constructor that creates a socket, a board object and GUI.
	 * @param port A integer for the port number that is going to be used for the server.
	 * @throws SocketException Throw exception if something went wrong creating the socket.
	 * @throws UnknownHostException Throw exception if the address is wrong.
	 */
	public Server(int port) throws SocketException, UnknownHostException {
		// Create a Datagram socket with port given as parameter and connect to IPv6
		// locahost address.
		this.socketUDP = new DatagramSocket(port,Inet6Address.getByAddress(new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
		// Inet6Address.getByAddress(new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1})
		this.board = new Board(this, 201, 201, 5);
		this.gui = new GUI(this.board);
		reader = new packetReader(socketUDP, board);
		thread = new Thread(reader);
		thread.start();
	}
	/**
	 * packetReader is a class that will be run as a thread and its purpose is managing
	 * the communication from client to server.
	 * @author Christoffer Rova
	 */
	public class packetReader implements Runnable {
		
		boolean runThread;
		Board board;
		DatagramSocket socketUDP;
		DatagramPacket packetUDP;
		byte[] dataUDP = new byte[4];
		/**
		 * This is the constructor that setups all the references to objects that is needed by the thread.
		 * @param socketUDP Reference to the already created datagram socket.
		 * @param board Reference to the object that manages the squares.
		 */
		public packetReader(DatagramSocket socketUDP, Board board) {
			this.socketUDP = socketUDP;
			this.packetUDP = new DatagramPacket(dataUDP, dataUDP.length);
			this.runThread = true;
			this.board = board;
		}
		/**
		 * This method decides what to do with the data received.
		 * @param data The byte array containing data.
		 */
		private void packetProcessor(byte[] data) {
			System.out.println(data);
			if(data[0] == Messages.CHANGE.ordinal()) {
				System.out.println("Changing color...");
				// Call the method setSquareColor in the Board object and give
				// coordinates and color integer as parameter.
				board.setSquareColor((int) data[1], (int) data[2], (int) data[3]);
				setChanged();
				notifyObservers();
			} else if(data[0] == Messages.RESET.ordinal()) {
				System.out.println("Resetting colors...");
				// Call the method resetSquares in the Board object to reset all
				// the squares.
				board.resetSquares();
				setChanged();
				notifyObservers();
			}
		}
		
		public void stopThread() {
			this.runThread = false;
		}
		/*
		 * (non-Javadoc)
		 * @see java.lang.Runnable#run()
		 */
		@Override
		public void run() {
			while(runThread) {
				// Try to receive data from the datagram socket, otherwise give an exception.
	            try {
					socketUDP.receive(packetUDP);
					dataUDP = packetUDP.getData();
					System.out.println(dataUDP);
		            System.out.println(dataUDP[0] + " " + dataUDP[1] + " " + dataUDP[2] + " " + dataUDP[3]);
		            // Send the received data to packetProcessor.
		            this.packetProcessor(dataUDP);
					setChanged();
					notifyObservers();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	}
}
