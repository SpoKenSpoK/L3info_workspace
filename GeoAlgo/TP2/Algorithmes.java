import java.awt.Color;
import java.util.Vector;

/** La classe Algorithmes. */
public class Algorithmes {
	
	/** Algorithme qui calcule le flot en fonction d'un ensemble de sommets et d'ar?tes. */ 
	static void algorithme(Vector<Sommet> sommets, Vector<Arete> aretes)
	{
		int couleur = 0;
		
		for(Arete arete : aretes )
		{
			// Affecte un flot
			arete.flot = arete.capacite / 2;
			
			// Affecte une couleur
			if ((couleur++) % 2 == 0)
				arete.couleur = Color.RED;
			else
				arete.couleur = Color.BLUE;
		}
	}
}
