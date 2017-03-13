import java.util.*;

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




    static Vector<Segment> quickHull(Vector<Point> points){
        Vector<Segment> pConvexe = new Vector<>();

        if(points.size() > 2){
            int indexLPoint = theLeftPoint(points);
            int indexRPoint = theRightPoint(points);

            Point lPoint = points.elementAt( indexLPoint );
            Point rPoint = points.elementAt( indexRPoint );

            Vector<Point> pointsMinusTwo = new Vector<>(points);

            pointsMinusTwo.remove(lPoint);
            pointsMinusTwo.remove(rPoint);

            Vector<Point> upSide = cutInTwoPart(pointsMinusTwo, lPoint, rPoint);
            Vector<Point> downSide = cutInTwoPart(pointsMinusTwo, rPoint, lPoint);

            findHull(pConvexe, upSide, lPoint, rPoint);
            findHull (pConvexe, downSide, rPoint, lPoint);
        }

        return pConvexe;
    }

    static Point farthestPoint(Vector<Point> points, Point pOne, Point pTwo){
        double aire;
        double demi_p;
        double edgeOne;
        double edgeTwo;
        double height;
        double maxHeight = -1;

        Point maxHeightPoint = new Point();

        double base = distanceTwoPts(pOne, pTwo);

        for(int i=0; i < points.size(); ++i){
            edgeOne = distanceTwoPts(pOne, points.elementAt(i));
            edgeTwo = distanceTwoPts(pTwo, points.elementAt(i));

            // Calcul de l'air du triangle dans un premier temps
            demi_p = ( edgeOne + edgeTwo + base ) / 2;
            aire = Math.sqrt( demi_p*
                    (demi_p - base)
                    *(demi_p - edgeOne)
                    *(demi_p - edgeTwo) );

            // Via l'air, il nous est possible de trouver la hauteur du triangle
            height = ( aire / ( base/2) );

            // Nous gardons la hauteur maximale trouvée
            if( height > maxHeight ){
                maxHeight = height;
                maxHeightPoint = points.elementAt(i);
            }
        }
        return maxHeightPoint;
    }

    static void findHull(Vector<Segment> pConvexe, Vector<Point> points, Point pOne, Point pTwo){
        if( points.size() < 1 ){
            pConvexe.add( new Segment(pTwo, pOne) );
            return;
        }

        Point maxHeightPoint = farthestPoint(points, pOne, pTwo);

        Vector<Point> pointsMinusThree = new Vector<>(points);

        pointsMinusThree.remove(pOne);
        pointsMinusThree.remove(pTwo);
        pointsMinusThree.remove(maxHeightPoint);

        Vector<Point> upSide = cutInTwoPart(pointsMinusThree, pOne, maxHeightPoint);
        Vector<Point> downSide = cutInTwoPart(pointsMinusThree, maxHeightPoint, pTwo);

        findHull(pConvexe, upSide, pOne, maxHeightPoint);
        findHull(pConvexe, downSide, maxHeightPoint, pTwo);
    }

    static Vector<Point> cutInTwoPart(Vector<Point> inPut, Point leftPoint, Point rightPoint){

        Vector<Point> outPut = new Vector<>();
        for(int i=0; i < inPut.size(); ++i){
            if( crossProduct(leftPoint, inPut.elementAt(i), rightPoint) )
                outPut.add( inPut.elementAt(i) );
        }

        return outPut;
    }

    static double distanceTwoPts(Point pOne, Point pTwo){
        return Math.sqrt( Math.pow((pTwo.x - pOne.x), 2) + Math.pow((pTwo.y - pOne.y), 2) );
    }

    static int theLeftPoint(Vector<Point> listePoints){
        int leftPoint = 0;

        for(int i=1; i < listePoints.size(); ++i)
            if( listePoints.elementAt(i).x < listePoints.elementAt(leftPoint).x )
                leftPoint = i;

        return leftPoint;
    }

    static int theRightPoint(Vector<Point> listePoints){
        int rightPoint = 0;

        for(int i=1; i < listePoints.size(); ++i)
            if( listePoints.elementAt(i).x > listePoints.elementAt(rightPoint).x )
                rightPoint = i;

        return rightPoint;
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
