public class Sum implements Operator {
    public String symbol(){ return "+"; }
    public double initalValue(){ return 0.0; }
    public double cumulativeValue(double acc, double value){ return (acc + value); }
}
