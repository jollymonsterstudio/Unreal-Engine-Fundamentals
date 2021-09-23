// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE4Fundamentals13/UE4Fundamentals13GameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUE4Fundamentals13GameMode() {}
// Cross Module References
	UE4FUNDAMENTALS13_API UClass* Z_Construct_UClass_AUE4Fundamentals13GameMode_NoRegister();
	UE4FUNDAMENTALS13_API UClass* Z_Construct_UClass_AUE4Fundamentals13GameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_UE4Fundamentals13();
// End Cross Module References
	void AUE4Fundamentals13GameMode::StaticRegisterNativesAUE4Fundamentals13GameMode()
	{
	}
	UClass* Z_Construct_UClass_AUE4Fundamentals13GameMode_NoRegister()
	{
		return AUE4Fundamentals13GameMode::StaticClass();
	}
	struct Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_UE4Fundamentals13,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "UE4Fundamentals13GameMode.h" },
		{ "ModuleRelativePath", "UE4Fundamentals13GameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUE4Fundamentals13GameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::ClassParams = {
		&AUE4Fundamentals13GameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AUE4Fundamentals13GameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AUE4Fundamentals13GameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AUE4Fundamentals13GameMode, 2166163759);
	template<> UE4FUNDAMENTALS13_API UClass* StaticClass<AUE4Fundamentals13GameMode>()
	{
		return AUE4Fundamentals13GameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AUE4Fundamentals13GameMode(Z_Construct_UClass_AUE4Fundamentals13GameMode, &AUE4Fundamentals13GameMode::StaticClass, TEXT("/Script/UE4Fundamentals13"), TEXT("AUE4Fundamentals13GameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AUE4Fundamentals13GameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
