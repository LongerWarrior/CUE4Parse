using CUE4Parse.UE4.Assets.Readers;
using CUE4Parse.UE4.Objects.Core.Math;
using CUE4Parse.UE4.Versions;
using Newtonsoft.Json;

namespace CUE4Parse.UE4.Assets.Exports.Component
{
    public class USceneComponent : UActorComponent
    {
        public bool bComputeBoundsOnceForGame;
        public FBoxSphereBounds? Bounds;
        public override void Deserialize(FAssetArchive Ar, long validPos)
        {
            base.Deserialize(Ar, validPos);
            bComputeBoundsOnceForGame = GetOrDefault<bool>(nameof(bComputeBoundsOnceForGame));
            if (bComputeBoundsOnceForGame)
            {
                if(FUE5PrivateFrostyStreamObjectVersion.Get(Ar) >= FUE5PrivateFrostyStreamObjectVersion.Type.SerializeSceneComponentStaticBounds)
                {
                    var bIsCooked = Ar.ReadBoolean();
                    if (bIsCooked)
                    {
                        Bounds = new FBoxSphereBounds(Ar);
                    }
                }
            }
        }

        protected internal override void WriteJson(JsonWriter writer, JsonSerializer serializer)
        {
            base.WriteJson(writer, serializer);

            if (Bounds is null) return;
            writer.WritePropertyName("Bounds");
            serializer.Serialize(writer, Bounds);
        }
    }
}
