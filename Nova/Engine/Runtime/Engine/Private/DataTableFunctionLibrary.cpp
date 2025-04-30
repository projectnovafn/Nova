#include "Kismet/DataTableFunctionLibrary.h"

float UDataTableFunctionLibrary::EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY)
{
	struct
	{
		UCurveTable* CurveTable; 
		FName RowName;
		float InXY;
		EEvaluateCurveTableResult OutResult;
		float OutXY;
		FString ContextString;
	} params(CurveTable, RowName, InXY);

	GetDefaultObject<UDataTableFunctionLibrary>()->ProcessEvent(L"EvaluateCurveTableRow", &params);

	return params.OutXY;
}