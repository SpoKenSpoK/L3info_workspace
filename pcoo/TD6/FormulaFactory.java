public class FormulaFactory {
    Formula createConstant(double value){
        return new Constant(value);
    }

    Forumla createSum(Formula ... operands){
        return new extension.Sum(operands);
    }

    Formula createProduct(Formula ... operands){
        return new extension.Product(operands);
    }
}
