import java.awt.Color;

/** La classe ar?te. */
public class Arete 
{
	/** L'extr?mite a. */
	public Sommet a;
	
	/** L'extr?mite b. */
	public Sommet b;
	
	/** La capacit?. */
	double capacite;
	
	/** Le flot. */
	double flot;
	
	/** La couleur. */
	Color couleur = Color.BLUE;
	
	/** Constructeur avec initialisation de a et b. */
	public Arete(Sommet a, Sommet b)
	{
		this.a = a;
		this.b = b;
	}
	
	/** Constructeur avec initialisation de a, b et capacit?. */
	public Arete(Sommet a, Sommet b, double capacite )
	{
		this.a = a;
		this.b = b;
		this.capacite = capacite;
	}
	
	/** Constructeur sans initialisation. */
	public Arete(){}
}
