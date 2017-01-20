import java.awt.Dimension;
import java.awt.Frame;

import javax.swing.JFrame;

/** La classe principale. */
public class Main  {
	
	/** La m?thode main. */
	public static void main(String[] args) {
		// Construction de la fen?tre
		JFrame frame = new JFrame("Flot Max");
		
		// Construction de la zone d'affichage
		ZoneSaisirGrapheCapaciteAfficherFlot zoneAffichage = new ZoneSaisirGrapheCapaciteAfficherFlot();
		
		// Ajout de la zone d'affichage ? la fenetre
		frame.getContentPane().add(zoneAffichage);
		
		// Taille
		frame.setSize(new Dimension(400,400));

		// Plein ?cran
		frame.setExtendedState(frame.getExtendedState() | Frame.MAXIMIZED_BOTH);
		
		// Affichage de la fen?tre
		frame.setVisible(true);
		
		// Fermeture de la fen?tre
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}