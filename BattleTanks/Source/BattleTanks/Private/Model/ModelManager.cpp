

#include "BattleTanks.h"
#include "ModelManager.h"

UModelManager::UModelManager()
{
	EMPTY = NewObject<UJsonModel>();
	Models = LoadModels();
}

TMap<FString, UJsonModel*> UModelManager::LoadModels()
{
	TMap<FString, UJsonModel*> mModel;

	UE_LOG(LogTemp, Warning, TEXT("Loading Models"))
		IFileManager& FileManager = IFileManager::Get();
	TArray<FString> files;

	FString gameDir = FPaths::GameContentDir() + "/Model/";
	FString ext = TEXT("*.json");
	FileManager.FindFiles(files, *gameDir, *ext);
	for (auto& FileName : files)
	{
		UJsonModel* model = NewObject<UJsonModel>();
		model->LoadFromFile(gameDir + FileName);

		FString ModelName = FString(FileName);
		ModelName.RemoveFromEnd(FString(".json"));

		mModel.Add(ModelName, model);
	}
	return mModel;
}

UJsonModel* UModelManager::GetModel(FString ModelName)
{
	if (Models.Contains(ModelName))
		return Models[ModelName];
	return EMPTY;
}

void  UModelManager::PreloadModelAssets()
{
	UE_LOG(LogTemp, Warning, TEXT("Preloading Models Assets"))
	
	TSet<FString> MeshesToLoad;
	TSet<FString> MaterialToLoad;
	
	for (auto& Entry : Models)
	{
		Entry.Value->PreloadAssets(MeshesToLoad, MaterialToLoad);
	}

	for (auto Mesh : MeshesToLoad)
	{
		UE_LOG(LogTemp, Warning, TEXT("\tPreloading %s"), *Mesh)
		UJsonModel::GetStaticMesh(FName(*Mesh));
	}

	for (auto Material : MaterialToLoad)
	{
		UE_LOG(LogTemp, Warning, TEXT("\tPreloading %s"), *Material)
		UJsonModel::GetMaterial(FName(*Material));
	}
}



