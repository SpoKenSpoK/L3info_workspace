public class Product implements Operator {
    public String symbol(){ return "*"; }
    public double initialValue(){ return 1.0; }
    public double cumulativeValue(double acc, double value){ return (acc * value); }
}
