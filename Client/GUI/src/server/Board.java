package server;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JPanel;
/**
 * @author Christoffer Rova
 * @version 1.0
 * date 2020-10-09
 * This class is managing all the squares that can be colored on the board.
 */
public class Board extends JPanel implements Observer {

		Square[][] map;
		int boardXSize, boardYSize, squareSize;
		Server server;
		
		/**
		 * The constructor for this class that creates a two-dimensional array
		 * that is storing Square objects.
		 * @param server The reference to the server object.
		 * @param xSize The number of squares in width.
		 * @param ySize The number of squares in height.
		 * @param squareSize The size of squares in pixels.
		 */
		public Board(Server server, int xSize, int ySize, int squareSize) {
			this.server = server;
			server.addObserver(this);
			this.boardXSize = xSize;
			this.boardYSize = ySize;
			this.map = new Square[boardXSize][boardYSize];
			// Loop that populates the array with Square objects.
			for(int i = 0; i < boardXSize; i++) {
				for(int j = 0; j < boardYSize; j++) {
					this.map[i][j] = new Square();
				}
			}
			this.squareSize = squareSize;
			
			Dimension d = new Dimension(boardXSize*squareSize, boardYSize*squareSize);
			this.setMinimumSize(d);
			this.setPreferredSize(d);
			this.setMaximumSize(d);
			this.setBackground(Color.WHITE);
			this.setVisible(true);
		}
		/**
		 * This method take x and y coordinates and calls getColor() method in the Square object
		 * and returns the color which is stored as a integer.
		 * @param x X integer coordinate.
		 * @param y Y integer coordinate.
		 * @return Integer representing the color.
		 */
		public int getSquareColor(int x, int y) {
			return this.map[x][y].getColor();
		}
		
		/**
		 * setSquareColor takes x and y coordinate and a integer representing a color
		 * and stores is in the Square object by using the objects own setColor method.
		 * @param x X integer coordinate.
		 * @param y Y integer coordinate.
		 * @param color Integer representing the color.
		 */
		public void setSquareColor(int x, int y, int color) {
			//x = x + 98;
			//y = y + 98;
			this.map[x][y].setColor(color);
			System.out.println("Color " + color + " set at " + x + " " + y);
		}
		/**
		 * resetSquares loop through all the objects stored in the array "map" and calls
		 * the method setSquareColor that will use the Square objects own method setColor()
		 * with the integer value 0 which represents white/no color.
		 */
		public void resetSquares() {
			for(int i = 0; i < this.boardXSize; i++) {
				System.out.println("i: " + i);
				for(int j = 0; j < this.boardYSize; j++) {
					System.out.println("Reseting " + i + " " + j);
					this.setSquareColor(i, j, 0);
				}
			}
		}
		
		@Override
		public void update(Observable arg0, Object arg1) {
			// TODO Auto-generated method stub
			this.repaint();
		}
		
		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			this.paintSquares(g);
		}
		/**
		 * This method returns a valid Color.color depending on the value of the
		 * integer parameter color.
		 * @param color
		 * @return
		 */
		private Color determineColor(int color) {
			if(color == 0) {
				return Color.WHITE;
			} else if(color == 1) {
				return Color.BLACK;
			} else if(color == 2) {
				return Color.RED;
			} else if(color == 3) {
				return Color.GREEN;
			} else if(color == 4) {
				return Color.BLUE;
			} else if(color == 5) {
				return Color.YELLOW;
			} else if(color == 6) {
				return Color.PINK;
			} else if(color == 7) {
				return Color.MAGENTA;
			} else if(color == 8) {
				return Color.CYAN;
			}
			return Color.WHITE;
		}
		/**
		 * The method that is responsible for painting the squares on the board.
		 * It loops through the array and calling the method getSquareColor() with the 
		 * coordinates as parameters which in turn calls the method getColor() in the Square object.
		 * 
		 * The integer returned is then passed as a parameter to determineColor() which in turn
		 * returns a valid Color.color which is used to set the color of the square. "fillRect"
		 * then paints the square at the correct coordinate on the board with the correct size as
		 * given by the variable squareSize.
		 * 
		 * @param g Reference to the Graphic2d object.
		 */
		public void paintSquares(Graphics g) {
			for(int i = 0; i < this.boardXSize; i++) {
				for(int j = 0; j < this.boardYSize; j++) {
					int temp = getSquareColor(i, j);
					Color tempColor = determineColor(temp);
					g.setColor(tempColor);
					g.fillRect((this.squareSize)*i,
							(this.squareSize)*j,
							(this.squareSize),
							(this.squareSize));
				}
			}
		}
}
