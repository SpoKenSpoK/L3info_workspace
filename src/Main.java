import java.awt.Dimension;

import javax.swing.JFrame;

/** La classe principale. */
public class Main  {

    /** La methode main. */
    public static void main(String[] args) {
        // Construction de la fenetre
        JFrame frame = new JFrame("Saisir des points et afficher des segments en resultat");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Construction de la zone d'affichage
        ZoneSaisirPointsAfficherSegments zoneAffichage = new ZoneSaisirPointsAfficherSegments();

        // Ajout de la zone d'affichage a la fenetre
        frame.getContentPane().add(zoneAffichage);

        // Dimension de la zone d'affichage
        zoneAffichage.setPreferredSize(new Dimension(1080,720));

        // Resize autour de la zone d'affichage
        frame.pack();

        // Affichage de la fenetre
        frame.setVisible(true);
    }
}
