public inteface AbstractFormulaFactory{
    Formula createConstant(double v);
    Formula createSum(Forumla [] f);
    Formula createProduct(Formula [] f);
}
