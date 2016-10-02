public class Main {
    public static void main(String[] args){
        int indexMin = -3;
        int indexMax = 1;

        RangeArray rArray = new RangeArray(indexMin,indexMax);
        for(int index = rArray.getIndexMin(); index <= rArray.getIndexMax(); ++index)
            rArray.set(index, index*index);

        for(int index = rArray.getIndexMin(); index <= rArray.getIndexMax(); ++index)
            System.out.println(index + " -> " + rArray.get(index));

        System.out.println(rArray.rangeSize());
    }
}
