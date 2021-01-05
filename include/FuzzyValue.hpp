
namespace FuzzyLogicPCL
{
    class FuzzyValue
    {
        internal LinguisticVariable Lv;
        internal double Value;

        public FuzzyValue(LinguisticVariable _lv, double _value)
        {
            Lv = _lv;
            Value = _value;
        }
    }
}
