package server;

import java.awt.Color;
import java.awt.Container;
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

	}

	@Override
	public void update(Observable o, Object arg) {
		frame.repaint();
	}
}
