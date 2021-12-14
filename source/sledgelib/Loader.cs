namespace SledgeLib
{
    public delegate void InitDelegate();

    public class Loader
    {
        public static void Init()
        {
            Console.WriteLine("sledgelib init");
        }
    }
}