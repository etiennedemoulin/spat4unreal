// Fill out your copyright notice in the Description page of Project Settings.


#include "SpatRoom.h"

ASpatRoom::ASpatRoom()
{
}

void ASpatRoom::PostRegisterAllComponents()
{
	uniqueRoomID = GetUniqueID();
}