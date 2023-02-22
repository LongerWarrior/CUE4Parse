using CUE4Parse.UE4.Assets.Readers;
using CUE4Parse.UE4.Objects.Core.Misc;
using CUE4Parse.UE4.Objects.UObject;
using CUE4Parse.UE4.Versions;
using Newtonsoft.Json;

namespace CUE4Parse.UE4.Assets.Exports.Component
{
    public class UActorComponent : UObject
    {
        [JsonIgnore]
        public FSimpleMemberReference[]? UCSModifiedProperties;
        public override void Deserialize(FAssetArchive Ar, long validPos)
        {
            base.Deserialize(Ar, validPos);
            if (FFortniteReleaseBranchCustomObjectVersion.Get(Ar) >= FFortniteReleaseBranchCustomObjectVersion.Type.ActorComponentUCSModifiedPropertiesSparseStorage)
	        {
                Ar.SkipFixedArray(28);//Just skip for now UCSModifiedProperties
                //UCSModifiedProperties = Ar.ReadArray(() => new FSimpleMemberReference(Ar));
		    }
        }
    }

    public struct FSimpleMemberReference
    {
        public FPackageIndex MemberParent;
        public FName MemberName;
        public FGuid MemberGuid;

        public FSimpleMemberReference(FAssetArchive Ar)
        {
            MemberParent = new FPackageIndex(Ar);
            MemberName = Ar.ReadFName();
            MemberGuid = Ar.Read<FGuid>();
        }
    }
}