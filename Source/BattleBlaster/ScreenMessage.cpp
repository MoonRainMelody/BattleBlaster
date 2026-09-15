// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	if (TextBlock) // 加上指针判空，防止编辑器里没绑定导致崩溃
	{
		TextBlock->SetText(FText::FromString(Message));
	}
}