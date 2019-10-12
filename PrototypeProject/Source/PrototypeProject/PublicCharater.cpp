// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicCharater.h"

// Sets default values
APublicCharater::APublicCharater()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stat.MaxHp = 100.f;
	Stat.Hp = Stat.MaxHp;
	Stat.IsMovable = true;
	Stat.MaxStack = 10;
	Stat.Stack = 0;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh(TEXT("SkeletalMesh'/Game/Animation/PC.PC'"));	
	if (BodyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
	}

	GetCapsuleComponent()->SetCapsuleSize(15.f, 45.f);
	GetMesh()->SetRelativeLocation(FVector(-5.f, 0.f, -45.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sword(TEXT("StaticMesh'/Game/sword/sword.sword'"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, false),
		TEXT("SwordSocket"));

	if (sword.Succeeded())
	{
		WeaponMesh->SetStaticMesh(sword.Object);
	}

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollsionSphere"));
	CollisionCapsule->SetCapsuleSize(3.f, 20.f);
	CollisionCapsule->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, false),
		TEXT("SwordSocket"));
	CollisionCapsule->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	CollisionCapsule->SetGenerateOverlapEvents(true);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APublicCharater::OnHitCollision);
	CollisionCapsule->SetGenerateOverlapEvents(false);

	CurrentState = EAnimationState::E_IDLE;

	GetCharacterMovement()->MaxWalkSpeed = 140.f;
	GetCharacterMovement()->MaxStepHeight = 12.f;
}

// Called when the game starts or when spawned
void APublicCharater::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APublicCharater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentState == EAnimationState::E_ATTACK ||
		CurrentState == EAnimationState::E_CHARGING ||
		CurrentState == EAnimationState::E_SKILL)
	{
		AddMovementInput(GetControlRotation().Vector(), 0.7f);
	}
}



void APublicCharater::PlusDestory()
{
	DestoryCount++;
}

// Called to bind functionality to input
void APublicCharater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APublicCharater::OnHitCollision(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void APublicCharater::EnableSwordOverlap(bool isEnable)
{
	CollisionCapsule->SetGenerateOverlapEvents(isEnable);
}

void APublicCharater::EndMotion()
{
	CurrentState = EAnimationState::E_IDLE;
}

void APublicCharater::PlayEffect(FVector loc, FRotator rot)
{
	GetWorld()->SpawnActor<AActor>(PaticleActor, loc, rot);
}

void APublicCharater::PlayEffect(APublicCharater * target)
{
	GetWorld()->SpawnActor<AActor>(PaticleActor, target->GetMesh()->GetSocketLocation(TEXT("Bip001-Spine2")), FRotator::ZeroRotator);
}

