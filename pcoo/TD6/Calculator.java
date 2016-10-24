public class Calculator {

    public static void main(String[] args){
        Calculator calculator = new Calculator();
        Formula formula = cal.analyze(args);
        System.out.println(formula.asString());
        System.out.println(formula.asValue());
    }

    /***** TRUC EN BORDEL RIEN COMPRIS ***/
    private FormulaFactory factory;
    public Calculator(){
        factory = new FormulaFactory;
    }
    /****/

    private Stack<Formula> stack;

    public Caclulator(){ stack = new Stack<Formula>(); }

    public Formula analyze(String[] tokens){
        assert stack.isEmpty();
        for(String token : tokens){
            analyzeToken(token);
        }
        if(stack.size() != 1) throw new RuntimeException("missing operator");

        Forumla result = stack.pop();
        return result;
    }

    private void analyzeToken(String token){
        if(token.equals("+")) analyzeSun();
        else if(token.equals("*")) analyzeProduct();
        else analyzeDouble();
    }

    private void analyzeDouble(String token){
        try{
            double value = Double.parseDouble(token);
        }
        catch(NumberFormatException e){
             throw RuntimeException("illegal token");
        }

        Constant const = new Constant(value); // instanciation directe ICI
        stack.push(const);
    }

    private void analyzeSum(){
        if(stack.size() < 2) throw new RuntimeException("missing operand for +");
        Forumla right = stack.pop();
        Formula left = stack.pop();
        Formula sum = new Sum(left, right);

        // c = factory.createSum(left, right);
        stack.push(sum);
    }

    private void analyzeProduct(){
        if(stack.size() < 2) throw new RuntimeException("missing operand for *");
        Forumla right = stack.pop();
        Formula left = stack.pop();
        Formula product = new Product(left, right);
        stack.push(product);
    }
}
