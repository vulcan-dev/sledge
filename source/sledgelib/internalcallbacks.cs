namespace SledgeLib
{
    internal class InternalCallbacks
    {
        internal static dUIntCallback cb_StateChangeFunc = new dUIntCallback((uint iState) =>
        {
            if (iState == (uint)EGameState.Playing)
            {
                Scene._ResetSounds();
            }
        });

        internal static CCallback? cb_StateChange;
    }
}