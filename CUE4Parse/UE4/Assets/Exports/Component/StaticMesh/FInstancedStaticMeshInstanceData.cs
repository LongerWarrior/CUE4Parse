using System.Diagnostics;
using CUE4Parse.UE4.Assets.Readers;
using CUE4Parse.UE4.Objects.Core.Math;
using CUE4Parse.UE4.Objects.UObject;
using CUE4Parse.UE4.Readers;
using CUE4Parse.UE4.Versions;

namespace CUE4Parse.UE4.Assets.Exports.Component.StaticMesh
{
    public class FInstancedStaticMeshInstanceData
    {
        private readonly FMatrix Transform; // don't expose the raw matrix for now

        public readonly FTransform TransformData = new();
        public readonly FPackageIndex[]? wOverrideMaterials;

        public FInstancedStaticMeshInstanceData(FAssetArchive Ar)
        {
            Transform = new FMatrix(Ar);

            if(Ar.Game == EGame.GAME_HogwartsLegacy)
            {
                wOverrideMaterials = Ar.ReadArray(() => new FPackageIndex(Ar));
            }
            TransformData.SetFromMatrix(Transform);
        }

        public override string ToString()
        {
            return TransformData.ToString();
        }
    }
}
