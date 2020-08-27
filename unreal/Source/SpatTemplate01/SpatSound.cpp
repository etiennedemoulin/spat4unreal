// Fill out your copyright notice in the Description page of Project Settings.


#include "SpatSound.h"

ASpatSound::ASpatSound()
{

}

ASpatSound::PostRegisterAllComponents()
{
	uniqueID = GetUniqueID();
}