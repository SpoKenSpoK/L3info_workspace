import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.QuadCurve2D;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

/** La classe ZoneSaisirGrapheCapaciteAfficherFlot. */
public class ZoneSaisirGrapheCapaciteAfficherFlot extends JPanel  {

	/** Cr?ation de la zone d'affichage. */
	public ZoneSaisirGrapheCapaciteAfficherFlot()
	{
		// Le canvas d'affichage
		final CanvasSaisirGrapheCapaciteAfficherFlot canvas = new CanvasSaisirGrapheCapaciteAfficherFlot(); 
		
		// Panel des boutons
		JPanel panelBoutons = new JPanel();
		
		// Cr?ation de la zone de saisie de la capacit? 
		final JTextField textCapacite = new JTextField("50");
		textCapacite.setColumns(5);
		
		// On fixe la capacit?
		canvas.capacite = 50;
	
		// Actualisation ? chaque keyReleased
		textCapacite.addKeyListener(new KeyAdapter(){
			public void keyReleased(KeyEvent arg0) {
				if (textCapacite.getText().length() != 0)
					canvas.capacite = Integer.parseInt(textCapacite.getText());
			}			
		});
		
		// Cr?ation du bouton Effacer
		JButton effacer = new JButton("Effacer");
		
		// Action du bouton Effacer
		effacer.addActionListener( new ActionListener(){
				public void actionPerformed(ActionEvent evt) {
					// Suppression des points et des ar?tes
					canvas.sommets.removeAllElements();
					canvas.aretes.removeAllElements();
					canvas.repaint();
				}
			}
		);
		
		// Ajout des boutons au panel panelBoutons
		panelBoutons.add(effacer);
		panelBoutons.add(new JLabel(" Capacit?"));
		panelBoutons.add(textCapacite);
		
		// Cr?ation du layout pour la position des panels
		setLayout(new BorderLayout());
		
		// Ajout du canvas au centre
		add(canvas, BorderLayout.CENTER);
		
		// Ajout des boutons au nord
		add(panelBoutons, BorderLayout.SOUTH);
	}
}

/** La classe CanvasSaisirGrapheCapaciteAfficherFlot. */
class CanvasSaisirGrapheCapaciteAfficherFlot extends JPanel implements MouseListener, MouseMotionListener {
	
	/** La liste des sommets. */
	Vector<Sommet> sommets;
	
	/** La liste des ar?tes. */
	Vector<Arete> aretes;
	
	/** Le num?ro du sommet s?lectionn?. */
	private int numSelectedPoint;
	
	/** La couleur d'un sommet. */
	private final Color pointColor = Color.GRAY;
	
	/** La couleur d'un sommet s?lectionn?. */
	private final Color selectedPointColor = Color.RED;

	/** La taille d'un point ? l'ecran. */
	private final int POINT_SIZE = 5;
	
	/** Extr?mit? a. */
	Sommet extermiteA;
	
	/** La capacit? lors de l'ajout d'une ar?te. */
	double capacite = 0;
	
	/** Position de la souris. */
	private int sx;
	private int sy;
	
	/** Le num?ro du dernier bouton cliqu?. */
	private int clicButton;
	
	
	/** Cr?ation de la zone d'affichage. */
	public CanvasSaisirGrapheCapaciteAfficherFlot()
	{
		// Initialisation de l'extr?mit? A
		extermiteA = null;
		
		// Cr?ation du vecteur de sommets
		sommets = new Vector<Sommet>();
		
		// Cr?ation du vecteur d'ar?tes
		aretes = new Vector<Arete>();
		
		// Initialisation de l'ar?te s?lectionn?e
		numSelectedPoint = -1;
		
		// Initialisation de la couleur de fond
		setBackground(Color.WHITE);
		
		// Ajout de la gestion des actions souris
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
	}
	
	/** Dessin de la zone d'affichage. */
	public void paint(Graphics g) {
		// Activation de l'anti-aliasing
		((Graphics2D)g).setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// Efface le fond
		g.clearRect(0,0,getWidth(),getHeight());
		
		// Dessin des ar?tes 
		drawAretes(g);
		
		// Affichage de l'ar?te en construction
		if(extermiteA != null)
			g.drawLine((int)extermiteA.x,(int)extermiteA.y,sx,sy);
		
		// Dessin des sommets
		drawSommets(g);
		
		// Affichage de l'aide
		drawAide(g);	
	}

	/** Affichage de l'aide. */
	private void drawAide(Graphics g)
	{
		// Couleur du texte
		g.setColor(Color.BLUE);
		
		// Si aucun point s?lectionn?
		if ( numSelectedPoint == - 1)
		{
			// Si l'extr?mite A n'est pas selectionn?e
			if ( extermiteA == null )
				g.drawString( "Clic gauche : ajouter un sommet", 10, 15 );
			else
			{
				g.drawString( "Cliquer sur l'extr?mit? b", 10, 15 );
				g.drawString( "Clic droit : annulation de la cr?ation de l'ar?te", 10, 30 );
			}
		}
		// Si un point s?lectionn?
		else
		{
			// Si construction d'une ar?te
			if (extermiteA != null)
			{
				if (numSelectedPoint != extermiteA.num)
				{
					g.drawString( "Clic gauche : s?lection de l'extr?mit? b", 10, 15 );
					g.drawString( "Clic milieu : changer la position du sommet", 10, 30 );
					g.drawString( "Clic droit : annulation de la cr?ation de l'ar?te", 10, 45 );
				}
				else
				{
					g.drawString( "Cliquer sur l'extr?mit? b", 10, 15 );
					g.drawString( "Clic milieu : changer la position du sommet", 10, 30 );
					g.drawString( "Clic droit : annulation de la cr?ation de l'ar?te", 10, 45 );
				}	
			}
			else
			{
				// Suppression d'un sommet possible si pas source et pas puits
				if (numSelectedPoint > 1)
				{
					g.drawString( "Clic gauche : s?lection de l'extr?mit? a", 10, 15 );
					g.drawString( "Clic milieu : changer la position du sommet", 10, 30 );
					g.drawString( "Clic droit : suppression du sommet", 10, 45 );
				}
				else
				{
					g.drawString( "Clic gauche : s?lection de l'extr?mit? a", 10, 15 );
					g.drawString( "Clic milieu : changer la position du sommet", 10, 30 );
				}		
			}
		}
	}
	
	/** Affichage des sommets. */
	private void drawSommets(Graphics g) {
		for (int n = 0; n < sommets.size(); n++) {
			Sommet p = sommets.elementAt(n);
			
			g.setColor(Color.WHITE);
			g.fillOval((int)(p.x - 2 * POINT_SIZE), (int)(p.y - 2 * POINT_SIZE), 2 * 2 * POINT_SIZE,	2 * 2 * POINT_SIZE);
			
			if ( n == numSelectedPoint ) 
				g.setColor(selectedPointColor);
			else
				g.setColor(pointColor);
			
			g.drawOval((int)(p.x - 2 * POINT_SIZE), (int)(p.y - 2 * POINT_SIZE), 2 * 2 * POINT_SIZE,	2 * 2 * POINT_SIZE);
			
			int l = g.getFontMetrics().stringWidth("" + p.num);
			int h = g.getFontMetrics().getHeight();
			
			if ( n == 0 )
				g.drawString( "S",(int)(p.x - (l/2)), (int)(p.y + (h/4)));
			else if ( n == 1)
				g.drawString( "P",(int)(p.x - (l/2)), (int)(p.y + (h/4)));
			else
				g.drawString( "" + p.num,(int)(p.x - (l/2)), (int)(p.y + (h/4)));	
		}
	}

	/** Affichage des ar?tes. */
	private void drawAretes(Graphics g) {
		for ( int n = 0; n < aretes.size(); n++ ) {
			Arete arete = aretes.elementAt(n);
			g.setColor(arete.couleur);
			drawArete( (Graphics2D)g, arete, 25, 12, 0.5 );
		}
	}
	
	/** Affichage d'une ar?te. */
	private void drawArete( Graphics2D g, Arete arete, double w, double h, double t)
	{
		// Calcul de delta x
		double dx = arete.b.x - arete.a.x;
		
		// Calcul de delta y
		double dy = arete.b.y - arete.a.y;
		
		// Calcul de la distance
		double d = Math.sqrt( dx * dx + dy * dy );
		
		// Pas d'affichage si petite distance entre les sommets
		if ( d < 5 )
			return;
		
		// Calcul de l'angle
		double angle = Math.acos( dx / d );
		if ( dy <= 0 ) angle = - angle;
		
		// Translation et rotation afin d'aligner a et b horizontalement et avoir a comme origine
		g.translate( arete.a.x, arete.a.y );
		g.rotate( angle );
		
		// Trace la fl?che
		
		// La courbe de la fl?che
		QuadCurve2D quadCourbe = new QuadCurve2D.Double( 0, 0, d/2, w, d, 0 );
		g.draw(quadCourbe);
		
		// La pointe de la fl?che
		g.drawLine((int)Math.round(d/2),(int)Math.round(w/2),(int)Math.round(d/2-h),(int)Math.round(w/2+(h-3)));
		g.drawLine((int)Math.round(d/2),(int)Math.round(w/2),(int)Math.round(d/2-h),(int)Math.round(w/2-(h-3)));
		
		// Affiche la capacit? et le flot en fonction de la pente
		if (dx > 0)
			g.drawString(flotToString(arete), (int)Math.round(d/2), (int) w);
		else
		{
			int tailleFont = g.getFontMetrics().charsWidth(flotToString(arete).toCharArray(),0,flotToString(arete).length() - 1);
			g.translate(d/2,w);
			g.rotate(Math.PI);
			g.drawString( flotToString(arete), Math.round(-(tailleFont+h)),g.getFontMetrics().getHeight()/2);
			g.rotate(-Math.PI);
			g.translate(-d/2,-w);
		}
		
		// Annulation de la translation et de la rotation
		g.rotate(-angle);
		g.translate(-arete.a.x, -arete.a.y);
	}

	/** Chaine repr?sentant le flot et la capacit?. */
	String flotToString(Arete arete)
	{
		String chaine = "";
		
		if ( arete.flot == Math.round(arete.flot))
			chaine += "f=" + Math.round(arete.flot);
		else
			chaine += "f=" + arete.flot;
		
		if ( arete.capacite == Math.round(arete.capacite))
			chaine += " c=" + Math.round(arete.capacite);
		else
			chaine += " c=" + arete.capacite;

		return chaine;
	}
	
	/** Retourne le num?ro du point situ? en (x,y). */
	private int getNumSelectedPoint(int x, int y) {
		
		for(int n = 0; n < sommets.size(); n++)
		{
			Sommet p = sommets.elementAt(n);
			
			if
			(
				p.x > x - 2 * POINT_SIZE && 
				p.x < x + 2 * POINT_SIZE &&
				p.y > y - 2 * POINT_SIZE && 
				p.y < y + 2 * POINT_SIZE
			)
				return n;
		}
		
		return -1;
	}

	/** Ajout d'un sommet en (x,y). */
	private void addSommet(int x, int y)
	{
		sommets.addElement(new Sommet(sommets.size(), x, y));
	}
	
	/** Suppression d'un sommet. */
	private void removeSommet(int numSelectedPoint)
	{
		Sommet s = sommets.elementAt(numSelectedPoint);
	
		// Supression des ar?tes adjacentes
		for(int a = 0; a < aretes.size(); a++)
		{
			Arete arete = aretes.elementAt(a);
			
			if (arete.a == s || arete.b == s)
			{
				aretes.removeElement(arete);
				a--;
			}
		}
		
		// Suppression du sommet
		sommets.removeElementAt(numSelectedPoint);
		
		// Nouvelle num?rotation
		for(int num = 0; num < sommets.size(); num++)
			sommets.elementAt(num).num = num;
		
	}
	
	/** Ajout d'une ar?te. */
	private void addArete(Sommet a, Sommet b)
	{
		for( Arete arete : aretes )
			if ( arete.a == a && arete.b == b )
				return;
		
		aretes.addElement(new Arete(a,b,capacite));
	}
	
	/** Gestion du clic souris. */
	public void mousePressed(MouseEvent evt) {
		
		// R?cup?ration du num?ro du clic
		clicButton = evt.getButton();
		
		if ( evt.getButton() == 1 )
		{
			// Si aucun sommet selectionn? et pas en cr?ation d'ar?te alors ajout du sommet
			if (numSelectedPoint == -1 && extermiteA == null)
			{
				addSommet(evt.getX(), evt.getY());
				numSelectedPoint = sommets.size() - 1;
				calculer();
				repaint();
			}
			// Si un sommet selectionn? alors validation de l'extr?mite A ou ajout d'une ar?te
			else if (numSelectedPoint != -1)
			{
				if( extermiteA == null )
					extermiteA = sommets.elementAt(numSelectedPoint);
				else
				{
					if ( sommets.elementAt(numSelectedPoint).num !=  extermiteA.num )
					{
						addArete(extermiteA, sommets.elementAt(numSelectedPoint));
						extermiteA = null;
						calculer();
					}
				}
				
				repaint();
			}
		}
		else if ( evt.getButton() == 3 )
		{
			// Si sommet s?lectionn? et pas construction ar?te alors suppression du sommet
			if (numSelectedPoint != -1 && extermiteA == null)
			{
				// Pas possible de supprimer la source et le puits
				if (numSelectedPoint > 1)
				{
					removeSommet(numSelectedPoint);
					numSelectedPoint = getNumSelectedPoint(evt.getX(), evt.getY());
					calculer();
					repaint();
				}
			}
			// Annulation
			else if (extermiteA != null)
			{
				extermiteA = null;
				repaint();
			}
		}
	}
	
	/** Le x et y du point numSelectedPoint est modifi? si
	 * 	la souris change de position avec le bouton 2 enfonc?.
	 */
	public void mouseDragged(MouseEvent evt) {
		sx = evt.getX();
		sy = evt.getY();
		
		if (numSelectedPoint != -1 && clicButton == 2)
		{
			sommets.elementAt(numSelectedPoint).x = evt.getX();
			sommets.elementAt(numSelectedPoint).y = evt.getY();
			calculer();
			repaint();
		}
	}
	
	/** Le numSelectedPoint est calcul? si
	 * 	la souris change de position sans bouton enfonc?.
	 */
	public void mouseMoved(MouseEvent evt) {
		sx = evt.getX();
		sy = evt.getY();
		
		numSelectedPoint = getNumSelectedPoint(evt.getX(), evt.getY());
		repaint();
	}
	
	/** Lance l'algorithme sur l'ensemble des sommets et ar?tes. */
	public void calculer()
	{
		Algorithmes.algorithme(sommets,aretes);
	}
	
	public void mouseReleased(MouseEvent evt) {}
	public void mouseEntered(MouseEvent evt) {}
	public void mouseExited(MouseEvent evt) {}
	public void mouseClicked(MouseEvent evt) {}
}