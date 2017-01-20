/** La classe Point. */
public class Point
{
	/** La valeur de x. */
	public double x;
	
	/** La valeur de y. */
	public double y;
	
	/** Constructeur avec initialisation de x et y. */
	public Point(double x, double y)
	{
		this.x = x;
		this.y = y;
	}
	
	/** Constructeur sans initialisation. */
	public Point(){}
	
	/** Position "graphique" du point dans le graph **/
	public Boolean isRight = true;
	
	public int number;
}