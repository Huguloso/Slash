// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define DRAW_SPHERE(LOCATION) if (GetWorld()) DrawDebugSphere(GetWorld(), LOCATION, 20, 30, FColor::Red);

