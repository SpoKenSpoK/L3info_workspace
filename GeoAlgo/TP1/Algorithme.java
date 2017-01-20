import java.util.Random;
import java.util.Vector;
import java.util.Collections;

/** La classe algorithme. */
class Algorithmes {
	
	/** Algorithme qui prend un ensemble de points et qui retourne un ensemble de segments. */ 
	static Vector<Segment> algorithme1(Vector<Point> points)
	{
		// Creation de la liste des segments
		Vector<Segment> segments = new Vector<Segment>();
		
		// Ajout d'un segment entre chaque paire de points consecutifs
		for(int n1 = 0; n1 < points.size() - 1; n1++)
		{
			Point p1 = points.elementAt(n1);
			Point p2 = points.elementAt(n1+1);
			segments.addElement(new Segment(p1,p2));
		}
		
		// Fermeture du polygône
		Point pFirst = points.get(0);
		Point pLast = points.lastElement();
		segments.addElement(new Segment(pLast, pFirst));
		
		tagPoints(points);
		sortPoints(points);
		
		return segments;
	}
	
	/** Retourne un nombre aleatoire entre 0 et n-1. */
	static int rand(int n)
	{
		int r = new Random().nextInt();
		
		if (r < 0) r = -r;
		
		return r % n;
	}
	
	static void tagPoints(Vector<Point> points){
		for(int i = 1; i < points.size(); ++i)
			if( points.elementAt(i).y < points.elementAt(i-1).y )
				points.elementAt(i).isRight = false;
	}
	
	static void sortPoints(Vector<Point> points){
		Vector<Point> rPoints = new Vector<Point>();
		Vector<Point> lPoints = new Vector<Point>();
		
		for(Point p : points){
			if(p.isRight) rPoints.add(p);
			else lPoints.add(p);
		}
		
		Collections.reverse(lPoints);
		Vector<Point> finalList = new Vector<Point>();
		
		for(;;){
			
			if(rPoints.isEmpty()){ finalList.addAll( lPoints ); break; }
			if(lPoints.isEmpty()){ finalList.addAll( rPoints ); break; }
			
			
			if( rPoints.firstElement().y < lPoints.firstElement().y ){
				finalList.add(rPoints.firstElement());
				rPoints.remove(rPoints.firstElement());
			}
			
			else{
				finalList.add(lPoints.firstElement());
				lPoints.remove(lPoints.firstElement());
			}
		}
		
		for(int i=0; i<finalList.size(); ++i){
			finalList.elementAt(i).number = i;
		}
	}
	
	
	static boolean crossProduct(Point pointOne, Point pointTwo, Point pointThree){
		double vecOne_x;
		double vecOne_y;
		
		double vecTwo_x;
		double vecTwo_y;
		
		vecOne_x = pointTwo.x - pointOne.x;
		vecOne_y = pointTwo.y - pointOne.y;
		
		vecTwo_x = pointTwo.x - pointThree.x;
		vecTwo_y = pointTwo.y - pointThree.y;
		
		return ((vecOne_x * vecTwo_y) - (vecTwo_x * vecOne_y)) < 180;
	}
	
}
