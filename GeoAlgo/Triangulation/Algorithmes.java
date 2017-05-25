import java.lang.reflect.Array;
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


    /* Intersection de segment */
    static Vector<Segment> segmentsIntersection(Vector<Point> points){
        Vector<Segment> segments = new Vector<>();

        for(int i=0; i < points.size()-1; i+=2){
            Segment newSeg;

            // Classement des points en Upper/Lower point dans notre segment :
            // a = Upper
            // b = Lower
            if( points.elementAt(i).y < points.elementAt(i+1).y )
                newSeg = new Segment( points.elementAt(i), points.elementAt(i+1) );
            else if( points.elementAt(i).y > points.elementAt(i+1).y )
                newSeg = new Segment( points.elementAt(i+1), points.elementAt(i) );
            else {
                if( points.elementAt(i+1).x < points.elementAt(i).x )
                    newSeg = new Segment(points.elementAt(i+1), points.elementAt(i));
                else
                    newSeg = new Segment(points.elementAt(i), points.elementAt(i+1));
            }

            newSeg.number = i/2;
            segments.add(newSeg);

        }

        // On lance une recherche d'intersection uniquement si le nombre total de points est modulo 2.
        if( points.size()%2 == 0 && points.size() > 2 )
            findIntersections(segments);

        return segments;
    }

    static class segmentCouple {
        public Segment segA;
        public Segment segB;

        public segmentCouple(Segment _segA, Segment _segB){
            segA = _segA;
            segB = _segB;
        }
    }

    /* Trouver les intersections entre tous les segments tracés */
    static void findIntersections(Vector<Segment> segments){

        // liste locale de segments que l'on peu supprimer.
        Vector<Segment> localSegments = new Vector<Segment>(segments);

        /*
            Entrée : tous les segments ont un point Upper(extrémité du haut) et Lower(extrémité du bas).
            Ces deux points, sont déjà triés suivant y & x

            Nous trions alors tous les segments de façon à ce qu'ils soient ordonnées du Upper le plus haut dans la fenêtre, vers le plus bas.
            Note : si deux segments ont la même coordonnée ̀y` pour leur UpperPoint, alors on les différencies suivant leur `x`

            On peut remplir alors une "pile" d'évènements à venir.
        */

        // TRIE A BULLE :
        // Ce tri permet notamment d'éviter des erreurs dans le cas où plusieurs
        // segment auraient le même point d'entrée (UpperPoint).
        boolean flag = false;
        while( flag != true ){

            int cpt = 0;
            int i = 0;

            for(i=0; i < localSegments.size()-1; ++i){

               if( localSegments.elementAt(i).a.y > localSegments.elementAt((i+1)).a.y ){
                   Collections.swap(localSegments,i+1 ,i);
                   ++cpt;
               }
               else if( localSegments.elementAt(i).a.y == localSegments.elementAt((i+1)).a.y ){
                   if( localSegments.elementAt(i).a.x > localSegments.elementAt((i+1)).a.x ){
                       Collections.swap(localSegments, i+1, i);
                       ++cpt;
                   }
               }
            }

            if( cpt == 0 ) flag = true;
        }

        // Fonction de comparaison pour notre TreeSet
        class SegmentCompare implements Comparator<Segment>{
            @Override
            public int compare(Segment one, Segment two) {
                if (one.a.x < two.a.x) return -1;
                if (one.a.x > two.a.x) return 1;
                return 0;
            }
        }

        TreeSet<Segment> tree = new TreeSet<Segment>(new SegmentCompare()); // Création de notre TreeSet:
        Iterator<Segment> iterator;  // Notre itérateur
        Vector<Segment> deleteEvent = new Vector<Segment>();


        Vector<segmentCouple> intersections = new Vector<segmentCouple>();

        Segment segMain;
        Segment segOne;
        Segment segTwo;

        System.out.println("-- START --");

        //DEBUG
        for(int i=0; i < localSegments.size(); ++i){
            System.out.print(localSegments.elementAt(i).number+" ");
        }
        System.out.println();

        double sweepLine = 0;
        while( localSegments.size() > 0 ){

            for(int i=0; i < localSegments.size(); ++i){
                // EVENT : Ajout d'un point
                // Ajout le nouveau segment
                // Check les intersections avec ces deux nouveaux voisins

                segMain = localSegments.elementAt(i);

                if( segMain.a.y == sweepLine ){
                    tree.add( segMain );

                    // CHECK FOR INTERSECTIONS
                   if( tree.size() > 1 ) {
                       // Insertion à l'extrémité gauche de l'arbre : -> first()
                       if (tree.first() == segMain) {
                           segOne = tree.higher(segMain);
                           if (doIntersect(segMain.a, segMain.b, segOne.a, segOne.b)) {
                               if (!existYet(intersections, new segmentCouple(segMain, segOne))) {
                                   intersections.add(new segmentCouple(segMain, segOne));
                               }
                           }
                       }

                       // Insertion à l'extrémité droite de l'arbre : -> last()
                       else if (tree.last() == segMain) {
                           segOne = tree.lower(segMain);

                           if (doIntersect(segMain.a, segMain.b, segOne.a, segOne.b)) {
                               if (!existYet(intersections, new segmentCouple(segMain, segOne))){
                                   intersections.add(new segmentCouple(segMain, segOne));
                               }
                           }
                       }

                       // Insertion entre deux autres voisins :
                       else {
                           segOne = tree.lower(segMain);
                           segTwo = tree.higher(segMain);

                           if (doIntersect(segMain.a, segMain.b, segOne.a, segOne.b)) {
                               if (!existYet(intersections, new segmentCouple(segMain, segOne))){
                                   intersections.add(new segmentCouple(segMain, segOne));
                               }
                           }


                           if (doIntersect(segMain.a, segMain.b, segTwo.a, segTwo.b)) {
                               if (!existYet(intersections, new segmentCouple(segMain, segOne))){
                                   intersections.add(new segmentCouple(segMain, segTwo));
                               }
                           }
                       }
                   }
                }

                if( segMain.b.y == sweepLine ){
                   if(tree.size() > 1){
                        if( segMain != tree.first() && segMain != tree.last() ){
                            segOne = tree.higher(segMain);
                            segTwo = tree.lower(segMain);

                            if( doIntersect( segOne.a, segOne.b, segTwo.a, segTwo.b) ){
                                if( !existYet(intersections, new segmentCouple(segOne, segTwo) ) ){
                                    intersections.add( new segmentCouple(segOne, segTwo));
                                }

                            }
                        }
                    }

                    tree.remove( localSegments.elementAt(i) );
                    deleteEvent.add( segMain );
                }
            }

            // En cas de suppression, on enlève le segment de la liste des segments.
            for(int i=0; i < deleteEvent.size(); ++i){
                localSegments.removeElement( deleteEvent.elementAt(i) );
            }
            deleteEvent.clear();

            ++sweepLine;
        }

        System.out.println("INTERSECTION(S):");
        for(int i=0; i < intersections.size(); ++i){
            System.out.println(" { "+ intersections.elementAt(i).segA.number + " & " + intersections.elementAt(i).segB.number + " }" );
        }

        System.out.println("-- END --");

        // ####
        // http://www.cdn.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
        // ####
    }

    static boolean existYet(Vector<segmentCouple> intersections, segmentCouple newInter){
        for(int i=0; i < intersections.size(); ++i){
            if( intersections.elementAt(i).segA.number == newInter.segA.number && intersections.elementAt(i).segB.number == newInter.segB.number )
                return true;

            if( intersections.elementAt(i).segA.number == newInter.segB.number && intersections.elementAt(i).segB.number == newInter.segA.number )
                return true;
        }
        return false;
    }


    static boolean liesOnLine(Point pCurrent, Point pOne, Point pTwo){
        double donex = pCurrent.x - pOne.x;
        double doney = pCurrent.y - pOne.y;

        double dtwox = pTwo.x - pOne.x;
        double dtwoy = pTwo.y - pOne.y;

        double cross = (donex * dtwoy) - (doney * dtwox);

        if(cross == 0) return true;
        return false;
    }


    // Technique pour la recherche d'intersections trouvée en ligne à l'adresse suivante :
    // ##### http://www.cdn.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ #####
    static boolean onSegment(Point p, Point q, Point r)
    {
        if (q.x <= Math.max(p.x, r.x) && q.x >= Math.min(p.x, r.x) && q.y <= Math.max(p.y, r.y) && q.y >= Math.min(p.y, r.y))
            return true;

        return false;
    }

    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are colinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    static int orientation(Point p, Point q, Point r)
    {
        // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
        // for details of below formula.
        double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;     // Segment colinéaire
        return (val > 0)? 1: 2;     // Segment non-colinéaire
    }

    // The main function that returns true if line segment 'p1q1' and 'p2q2' intersect.
    /*
        Tous les cas spéciaux sont expliqués en ligne sur le lien donné plus haut
     */
    static boolean doIntersect(Point p1, Point q1, Point p2, Point q2)
    {
        // Find the four orientations needed for general and
        // special cases
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        // General case
        if (o1 != o2 && o3 != o4)
            return true;

       // Special Cases
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;

        // p1, q1 and p2 are colinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;

        // p2, q2 and p1 are colinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;

        // p2, q2 and q1 are colinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;


        return false; // Doesn't fall in any of the above cases
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

    /* Triangulation */
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
