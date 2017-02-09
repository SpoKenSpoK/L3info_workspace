import java.util.Random;
import java.util.Stack;
import java.util.Vector;
import java.util.Collections;

/** La classe algorithme. */
class Algorithmes {
	/** Algorithme qui prend un ensemble de points et qui retourne un ensemble de segments. */ 
	static Vector<Segment> algorithme1(Vector<Point> points)
	{
		// Creation de la liste des segments
		Vector<Segment> segments = new Vector<Segment>();
		Segment newSeg;
		
		// Ajout d'un segment entre chaque paire de points consecutifs
		for(int n1 = 0; n1 < points.size() - 1; n1++)
		{
			Point p1 = points.elementAt(n1);
			Point p2 = points.elementAt(n1+1);
			
			newSeg = new Segment(p1,p2);
			newSeg.diagonale = false;
			segments.addElement(newSeg);
		}
		
		if( segments.size() > 1 ){
			// Fermeture du polygône
			Point pFirst = points.elementAt(0);
			Point pLast = points.lastElement();
			
			newSeg = new Segment(pFirst, pLast);
			newSeg.diagonale = false;
			segments.addElement(newSeg);
		}
		
		tagPoints(points);
		Vector<Point> finalList = sortPoints(points);
		triangulation(finalList, segments);	
		
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
		
		for(int i = 1; i < points.size(); ++i){
			if( points.elementAt(i).y < points.elementAt(i-1).y )
				points.elementAt(i).isRight = false;
			else
				points.elementAt(i).isRight = true;
		}
	}
	
	static Vector<Point> sortPoints(Vector<Point> points){
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
		
		return finalList;
	}
	
	
	static boolean crossProduct(Point pointOne, Point pointTwo, Point pointThree){
		double vecOne_x;
		double vecOne_y;
		
		double vecTwo_x;
		double vecTwo_y;
		
		vecOne_x = pointTwo.x - pointOne.x;
		vecOne_y = pointTwo.y - pointOne.y;
		
		vecTwo_x = pointThree.x - pointTwo.x;
		vecTwo_y = pointThree.y - pointTwo.y;
		
		return ((vecOne_x * vecTwo_y) - (vecTwo_x * vecOne_y)) > 0;
	}
	
	static void triangulation(Vector<Point> finalList, Vector<Segment> segments){
		if(finalList.size() < 2) return;
		
		// Création de notre pile S
		Stack<Point> pileS = new Stack<>();
		
		// Ajout des deux premiers sommets
		pileS.push(finalList.elementAt(0));
		pileS.push(finalList.elementAt(1));
		
		for(int i=2; i < finalList.size()-1; ++i){
			
			Point pCourrant = finalList.elementAt(i);
			
			if( pCourrant.isRight != pileS.lastElement().isRight ){ // CAS 1 - éléments sur chaînes différentes
				
				while( pileS.size() > 1 ){
					
					Segment newSeg = new Segment( pCourrant, pileS.pop() );
					newSeg.diagonale = true;
					segments.addElement( newSeg );
				}
				
				pileS.pop();
				
				pileS.push(finalList.elementAt(i-1));
				pileS.push(pCourrant);
			}
			
			else {	// CAS 2 - éléments sur même chaîne
				
				while( pileS.size() > 1 ){
					
					System.out.println("START PILE");
					for(int j=0; j < pileS.size(); ++j){
						System.out.println(pileS.elementAt(j).number);
					}
					System.out.println("END PILE");
					
					Point tetePile = pileS.pop(); // tête de pile
					Point tetePileAnt = pileS.pop(); // tête de pile - 1

					if(crossProduct( tetePileAnt, tetePile, pCourrant )){ // Produit vectoriel
						
						Segment newSeg = new Segment( pCourrant, tetePileAnt );
						newSeg.diagonale = true;
						segments.addElement( newSeg );
						
						pileS.push(tetePileAnt);
						pileS.push(pCourrant);
						
					}
					
					else{
						pileS.push(tetePileAnt);
						pileS.push(tetePile);
						pileS.push(pCourrant);
						break;
					}
				}
				
			}
		}
			
			if(pileS.size() > 1){
				// Ajout des dernières diagonales
				pileS.pop();
				
				System.out.println("----");
				while(pileS.size() > 1 ){
					
					System.out.print("->");
					System.out.println(pileS.lastElement().number);
					Segment newSeg = new Segment( finalList.lastElement(), pileS.pop() );
					newSeg.diagonale = true;
					segments.addElement( newSeg );
				}
			
			}
			
	}
	
}
