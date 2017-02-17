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


    static Vector<Segment> algoIncremental(Vector<Point> points){
        Vector<Segment> segments = new Vector<>();



        /*
            Input = a set S of n points
            Assume that there are at least 2 points in the input set S of points
            QuickHull (S)
            {
               // Find convex hull from the set S of n points
               Convex Hull := {}
               Find left and right most points, say A & B, and add A & B to convex hull
               Segment AB divides the remaining (n-2) points into 2 groups S1 and S2
                   where S1 are points in S that are on the right side of the oriented line from A to B,
                   and S2 are points in S that are on the right side of the oriented line from B to A
               FindHull (S1, A, B)
               FindHull (S2, B, A)
           }
           FindHull (Sk, P, Q)
           {
               // Find points on convex hull from the set Sk of points
               // that are on the right side of the oriented line from P to Q
               If Sk has no point, then return.
               From the given set of points in Sk, find farthest point, say C, from segment PQ
               Add point C to convex hull at the location between P and Q
               Three points P, Q, and C partition the remaining points of Sk into 3 subsets: S0, S1, and S2
                   where S0 are points inside triangle PCQ, S1 are points on the right side of the oriented
                   line from  P to C, and S2 are points on the right side of the oriented line from C to Q.
               FindHull(S1, P, C)
               FindHull(S2, C, Q)
           }
           Output = convex hull
         */

        return segments;
    }

    static int theLeftPoint(Vector<Point> listePoints){
        int index = 0;
        double leftPoint = listePoints.elementAt(0).x;

        for(int i=1; i < listePoints.size(); ++i){
            if( listePoints.elementAt(i).x < leftPoint ){
                index = i;
                leftPoint = listePoints.elementAt(i).x;
            }
        }

        return index;
    }

    static int theRightPoint(Vector<Point> listePoints){
        int index = 0;
        double rightPoint = listePoints.elementAt(0).x;

        for(int i=1; i < listePoints.size(); ++i){
            if( listePoints.elementAt(i).x > rightPoint ){
                index = i;
                rightPoint = listePoints.elementAt(i).x;
            }
        }

        return index;
    }

    /** Retourne un nombre aleatoire entre 0 et n-1. */
    static int rand(int n)
    {
        int r = new Random().nextInt();

        if (r < 0) r = -r;

        return r % n;
    }

    static void tagPoints(Vector<Point> points) {

        int min = 0;

        for(int i = 1; i < points.size(); ++i){
            if( points.elementAt(i).y <= points.elementAt(i-1).y )
                points.elementAt(i).isRight = false;
            else {
                points.elementAt(i).isRight = true;
                min = i;
            }
        }

        points.elementAt(min).isRight = true;

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

            if( rPoints.firstElement().y == lPoints.firstElement().y ){
                if( rPoints.firstElement().x > lPoints.firstElement().x ){
                    finalList.add(lPoints.firstElement());
                    lPoints.remove(lPoints.firstElement());
                    continue;
                }
                else{
                    finalList.add(rPoints.firstElement());
                    rPoints.remove(rPoints.firstElement());
                    continue;
                }
            }

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

        vecOne_x = pointOne.x - pointTwo.x;
        vecOne_y = pointOne.y - pointTwo.y;

        vecTwo_x = pointThree.x - pointTwo.x;
        vecTwo_y = pointThree.y - pointTwo.y;

        if( pointTwo.isRight ) return ( (vecOne_y * vecTwo_x)) - (vecOne_x * vecTwo_y) > 0;
        return ((vecOne_x * vecTwo_y) - (vecOne_y * vecTwo_x)) > 0;
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

                // On trace toutes les diagonales sauf le dernier élément de la pile
                while( pileS.size() > 1 ){
                    Segment newSeg = new Segment( pCourrant, pileS.pop() );
                    newSeg.diagonale = true;
                    segments.addElement( newSeg );
                }
                pileS.pop(); // Elimination du dernier élément non popé

                pileS.push(finalList.elementAt(i-1));
                pileS.push(pCourrant);
            }

            else {	// CAS 2 - éléments sur même chaîne
               while( pileS.size() > 1 ) {

                    Point tetePile = pileS.pop(); // tête de pile
                    Point tetePileAnt = pileS.pop(); // tête de pile - 1

                    if(crossProduct(tetePileAnt, tetePile, pCourrant)) { // Produit vectoriel
                        Segment newSeg = new Segment(pCourrant, tetePileAnt);
                        newSeg.diagonale = true;
                        segments.addElement(newSeg);

                        pileS.push(tetePileAnt);
                    }
                    else {
                        pileS.push(tetePileAnt);
                        pileS.push(tetePile);
                        break;
                    }
                }

                pileS.push(pCourrant);
            }
        }

        // Ajout des dernières diagonales
        pileS.pop(); // Tout tracer sauf le premier élément de la pile
        // Tout tracer sauf le dernier élément de la pile
        while( pileS.size() > 1 ){
            Segment newSeg = new Segment(finalList.lastElement(), pileS.pop());
            newSeg.diagonale = true;
            segments.addElement(newSeg);
        }
    }

}
