using System.Reflection;

internal class CSledgeUtils
{
    public static string? GetValidPath(string sPath, Assembly Caller)
    {
        if (File.Exists(sPath))
            return sPath;

        if (!Path.IsPathRooted(sPath))
        {
            if (File.Exists(Path.GetFullPath(sPath)))
                return Path.GetFullPath(sPath);

            if (Caller.FullName == null)
                return null;

            CModLoader.CModContext? ModInfo = CModLoader.RegisteredMods.Find(Info => Info.m_Assembly.FullName == Caller.FullName);

            if (ModInfo == null)
                return null;

            if (File.Exists(ModInfo.m_Path + "\\" + sPath))
                return ModInfo.m_Path + "\\" + sPath;
        }

        return null;
    }
}