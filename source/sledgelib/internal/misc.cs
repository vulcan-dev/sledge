using System.Text;

namespace SledgeLib
{
    public class Interop
    {
        internal static StringBuilder AllocString(nuint Length)
        {
            return new StringBuilder((int)Length);
        }
    }
}