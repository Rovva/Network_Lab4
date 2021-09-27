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
	
	public GUI(Board board) {
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

	}
	public void addMoveListener(KeyListener movelistener) {
		board.grabFocus();
		board.addKeyListener(movelistener);
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
    			        	//gameState.movePlayer(1);
    			        	direction = 1;
    			        } else if(keyCode == KeyEvent.VK_UP) {
    			        	System.out.println("UP");
    			        	//gameState.movePlayer(2);
    			        	direction = 2;
    			        } else if(keyCode == KeyEvent.VK_RIGHT) {
    			        	System.out.println("Right");
    			        	//gameState.movePlayer(3);
    			        	direction = 3;
    			        } else if(keyCode == KeyEvent.VK_DOWN) {
    			        	System.out.println("Down");
    			        	//gameState.movePlayer(4);
    			        	direction = 4;
    			        } else if(keyCode == KeyEvent.VK_SPACE) {
    			        	System.out.println("Hit");
    			        	direction = 5;
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
