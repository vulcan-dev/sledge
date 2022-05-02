namespace SledgeLib
{
    public enum EGameState : uint
    {
        Unknown,
        Splash,
        Menu,
        UI,
        Playing,
        Editing,
        Quitting
    }

    public enum EPaintType : uint
    {
        Spray = 0,
        Explosion = 1,
        Burnt = 2,
        NoSnow = 3,
        WeirdMat = 4
    }
}