// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TheTherapyCharacter.generated.h"

UCLASS(Blueprintable)
class ATheTherapyCharacter final : public ACharacter
{
  GENERATED_BODY()

public:
  ATheTherapyCharacter();

  auto addHeart() -> void;
  auto getHeartsCount() -> int;
  auto setDistanceToTheGoal(float) -> void;
  auto won() -> void;
  auto getObscuringWalls() -> std::vector<class AMaze *>;

  /** Returns TopDownCameraComponent subobject **/
  FORCEINLINE class UCameraComponent *GetTopDownCameraComponent() const
  {
    return TopDownCameraComponent;
  }
  /** Returns CameraBoom subobject **/
  FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }

  UFUNCTION(BlueprintCallable)
  void footStep();

  UFUNCTION(BlueprintCallable)
  bool isAlive() const;

  UFUNCTION(BlueprintCallable)
  bool isWin() const;

private:
  auto BeginPlay() -> void final;
  auto Tick(float DeltaSeconds) -> void final;

  /** Top down camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  class UCameraComponent *TopDownCameraComponent;

  /** Camera boom positioning the camera above the character */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  class USpringArmComponent *CameraBoom;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
  class UPointLightComponent *light;

  UFUNCTION()
  void onHit(AActor *self, AActor *other, FVector imp, const FHitResult &hit);

  int heartsCount = 0;
  class USoundCue *stepsSnd;
  class USoundCue *heartSnd;
  class USoundCue *deathSnd;

  bool m_isAlive;
  bool m_isWin;
};

