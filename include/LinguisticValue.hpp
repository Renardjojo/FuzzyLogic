using FuzzyLogicPCL.FuzzySets;
using System;

namespace FuzzyLogicPCL
{
    public class LinguisticValue
    {
        internal FuzzySet Fs { get; set; }
        internal String Name { get; set; }

        public LinguisticValue(String _name, FuzzySet _fs)
        {
            Name = _name;
            Fs = _fs;
        }

        internal double DegreeAtValue(double val)
        {
            return Fs.DegreeAtValue(val);
        }
    }
}
