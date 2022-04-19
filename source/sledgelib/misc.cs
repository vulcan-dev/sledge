using System.Text;

namespace SledgeLib
{
    public class Misc
    {
        internal static StringBuilder AllocString(nuint Length)
        {
            return new StringBuilder((int)Length);
        }
    }
}