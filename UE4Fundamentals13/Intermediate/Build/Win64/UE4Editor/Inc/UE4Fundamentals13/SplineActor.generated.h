// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FHitResult;
#ifdef UE4FUNDAMENTALS13_SplineActor_generated_h
#error "SplineActor.generated.h already included, missing '#pragma once' in SplineActor.h"
#endif
#define UE4FUNDAMENTALS13_SplineActor_generated_h

#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_SPARSE_DATA
#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execStopMovement); \
	DECLARE_FUNCTION(execTriggerMove); \
	DECLARE_FUNCTION(execMoveAlongTimeline); \
	DECLARE_FUNCTION(execOnTriggerEndOverlap); \
	DECLARE_FUNCTION(execOnTriggerBeginOverlap);


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execStopMovement); \
	DECLARE_FUNCTION(execTriggerMove); \
	DECLARE_FUNCTION(execMoveAlongTimeline); \
	DECLARE_FUNCTION(execOnTriggerEndOverlap); \
	DECLARE_FUNCTION(execOnTriggerBeginOverlap);


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASplineActor(); \
	friend struct Z_Construct_UClass_ASplineActor_Statics; \
public: \
	DECLARE_CLASS(ASplineActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/UE4Fundamentals13"), NO_API) \
	DECLARE_SERIALIZER(ASplineActor)


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_INCLASS \
private: \
	static void StaticRegisterNativesASplineActor(); \
	friend struct Z_Construct_UClass_ASplineActor_Statics; \
public: \
	DECLARE_CLASS(ASplineActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/UE4Fundamentals13"), NO_API) \
	DECLARE_SERIALIZER(ASplineActor)


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ASplineActor(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ASplineActor) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASplineActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASplineActor); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASplineActor(ASplineActor&&); \
	NO_API ASplineActor(const ASplineActor&); \
public:


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASplineActor(ASplineActor&&); \
	NO_API ASplineActor(const ASplineActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASplineActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASplineActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ASplineActor)


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_PRIVATE_PROPERTY_OFFSET
#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_13_PROLOG
#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_PRIVATE_PROPERTY_OFFSET \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_SPARSE_DATA \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_RPC_WRAPPERS \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_INCLASS \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_PRIVATE_PROPERTY_OFFSET \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_SPARSE_DATA \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_INCLASS_NO_PURE_DECLS \
	UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UE4FUNDAMENTALS13_API UClass* StaticClass<class ASplineActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID UE4Fundamentals13_Source_UE4Fundamentals13_Props_SplineActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
