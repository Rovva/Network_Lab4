package server;

import java.awt.Color;
import java.awt.Container;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet6Address;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Observable;
/**
 * @author Christoffer Rova
 * @version 1.0
 * date 2020-10-09
 * GUI is the class that handles the graphical interfaces which includes the 
 * object Board which in turn paints all the squares in the gui.
 */
public class GUI implements Observer {
	JFrame frame;
	JPanel panel;
	SpringLayout layout;
	Container contentPane;
	Board board;
	Server server;
	
	int x_size = 1050, y_size = 1050;
	
	DatagramSocket socketUDP;
	DatagramPacket packetUDP;
	byte[] dataUDP = new byte[4];
	
	public GUI(Board board) throws SocketException, UnknownHostException {
		this.board = board;
		
		frame = new JFrame("Color changer");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(x_size, y_size);
		frame.setVisible(true);
		layout = new SpringLayout();
		contentPane = frame.getContentPane();
		contentPane.setLayout(layout);
		contentPane.setBackground(Color.WHITE);
		
		this.contentPane.add(board);
		this.board.setFocusable(true);
		this.board.setVisible(true);

		this.layout.putConstraint(SpringLayout.NORTH, board, 0, SpringLayout.NORTH, contentPane);
		this.layout.putConstraint(SpringLayout.WEST, board, 0, SpringLayout.WEST, contentPane);
		this.initKeys();
		

		this.socketUDP = new DatagramSocket();
	}
	public void addMoveListener(KeyListener movelistener) {
		board.grabFocus();
		board.addKeyListener(movelistener);
	}
	
	public void sendUDP(byte[] data) throws IOException {
		System.out.println(Inet6Address.getByAddress(null, new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 4445).toString());
		this.packetUDP = new DatagramPacket(dataUDP, dataUDP.length, Inet6Address.getByAddress(new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}), 4445);
		this.packetUDP.setData(data);
		this.socketUDP.send(packetUDP);
		
	}
	
    private void initKeys() {
    	this.addMoveListener(
    			new KeyListener() {
    				@Override
    				public void keyPressed(KeyEvent ke) {
    			        int keyCode = ke.getKeyCode();
    			        int direction = 0;
    			        
    			        // Check if any of the pressed keys are valid.
    			        if(keyCode == KeyEvent.VK_LEFT) {
    			        	System.out.println("Left");
    			        	byte[] data = new byte[1];
    			        	data[0] = 4;
    			        	try {
								sendUDP(data);
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
    			        } else if(keyCode == KeyEvent.VK_UP) {
    			        	System.out.println("UP");
    			        	byte[] data = new byte[1];
    			        	data[0] = 8;
    			        	try {
								sendUDP(data);
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
    			        } else if(keyCode == KeyEvent.VK_RIGHT) {
    			        	System.out.println("Right");
    			        	byte[] data = new byte[1];
    			        	data[0] = 6;
    			        	try {
								sendUDP(data);
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
    			        } else if(keyCode == KeyEvent.VK_DOWN) {
    			        	System.out.println("Down");
    			        	byte[] data = new byte[1];
    			        	data[0] = 2;
    			        	try {
								sendUDP(data);
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
    			        }
			        	System.out.println("Sending stuff.");
						//controller.sendInput(direction);
    			      }

					@Override
					public void keyReleased(KeyEvent arg0) {
						// TODO Auto-generated method stub
						
					}

					@Override
					public void keyTyped(KeyEvent arg0) {
						// TODO Auto-generated method stub
						
					}
    			});
    }
	@Override
	public void update(Observable o, Object arg) {
		frame.repaint();
	}
}
