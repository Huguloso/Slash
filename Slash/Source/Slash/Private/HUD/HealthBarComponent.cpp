#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(const float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget)
	{
		 HealthBarWidget->HealthBar->SetPercent(Percent); 
	}
}
