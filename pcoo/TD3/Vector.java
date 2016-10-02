public class Vector<T> implements Iterable<T> {
    private class Iterator implements java.util.Iterator {

    }

    public void addAll(Vector<? extends T> other){
        for(T element : other)
            this.add(element);
    }

    public boolean isSorted(comparator <T> cmp){
        for(int right=1; right<size; ++right){
            int left = right - 1;
            if(cmp.compare(elements[left], elements[right]) > 0)
                return false;
        }

        return true;
    }


}

public inteface Comparator<T> {
    public boolean compare(T left, T right);
}

public class StringComparator implements Compatarator<String> {
    public int compare(String left, String right){
        return left.compareTo(right);
    }
}
