public inteface Operator {
    public String symbol();
    public double initialValue();
    public double cumulativeValue(double acc, double value);
}
