/** La classe Point. */
public class Sommet
{
	/** Num?ro du sommet. */
	int num;
	
	/** La valeur de x ? l'?cran. */
	public double x;
	
	/** La valeur de y ? l'?cran. */
	public double y;
	
	/** Constructeur avec initialisation de x et y. */
	public Sommet(int num, double x, double y)
	{
		this.num = num;
		this.x = x;
		this.y = y;
	}
	
	/** Constructeur sans initialisation. */
	public Sommet(){}
}