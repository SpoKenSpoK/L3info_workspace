public class RangeArray<T> implements Iterable<T> {
    private int indexMin;
    private int indexMax;
    private Object[] elements; // <= Object type générique, parents de tous les autres

    public RangeArray(int indexMin, int indexMax){
        assert (indexMin <= indexMax);
        this.indexMin = indexMin;
        this.indexMax = indexMax;
        elements = new Object[rangeSize()];
    }

    public void set(int userIndex, T value){
        assert userIndexIsValid(userIndex);
        int internalIndex = internalIndex(userIndex);
        elements[internalIndex] = value;
    }

    public boolean userIndexIsValid(int userIndex){ return (indexMin <= userIndex) && (userIndex <= indexMax); }
    public int internalIndex(int userIndex){ return userIndex - indexMin; }
    public int getIndexMax(){ return this.indexMax; }
    public int getIndexMin(){ return this.indexMin; }

    //@SuppressWarnings("uncheked"); // <= Supprime le warning une fois dans eclipse (IDE)
    public T get(int userIndex){
        assert userIndexIsValid(userIndex);
        int internalIndex = internalIndex(userIndex);
        return (T)elements[internalIndex];
    }

    public int rangeSize(){ return (indexMax - indexMin) + 1; }

    public Iterator<T> iterator(){ return new RangeArrayIterator(this); }

    // Classe interne
    public class RangeArrayIterator<T> implements Iterator<T> {
        private RangeArray<T> array;
        private int next;

        public RangeArrayIterator(RangeArray<T> array){
            this.array = array;
            next = array.getIndexMin();
        }

        public boolean hasNext(){
            return next <= array.getIndexMax();
        }

        public T next(){
            if(!hasNext()) throw new NoSuchElementException(" ... ");
            return array.get(nextIndex++);
        }
    }

    /**
        Vielle utilisation :

        Iterator<String> it = habitations.iterator();
        while(it.hasNext()){
            String h = it.next();
            System.out.println(h);
        }

        Nouvelle :

        for(string h : habitations){
            System.out.println(h);
        }
    */
}
