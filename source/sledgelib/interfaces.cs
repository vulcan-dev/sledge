namespace SledgeLib
{
    /*
     *  class used by mods to declare their information and load / unload methods
     */
    public interface ISledgeMod
    {
        public string GetName();
        public string GetDescription();
        public string GetVersion();
        public string GetAuthor();

        public void Load();
        public void Unload();
    }
}