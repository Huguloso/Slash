#pragma once

#define DRAW_SPHERE(LOCATION) if (GetWorld()) DrawDebugSphere(GetWorld(), LOCATION, 20, 30, FColor::Red, true);
#define DRAW_SPHERE_SingleFrame(LOCATION) if (GetWorld()) DrawDebugSphere(GetWorld(), LOCATION, 20, 30, FColor::Red, false);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), GetActorLocation(), 15, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), GetActorLocation(), 15, FColor::Red, false);
