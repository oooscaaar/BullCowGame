// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{

    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}


void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    // Welcoming the Player
    PrintLine(TEXT("Hi there! Welcome to BullCowGame :)"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives"), Lives);
    PrintLine(TEXT("Press enter to continue..."));
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord);
    //PrintLine(TEXT("The number of valid words is: %i"), GetValidWords(Words).Num());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("Congratulations, you have won!"));
        EndGame();
        return;
    }
    
    // Check Right Number Of Character
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }

    // Check if isogram
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letter, guess again"));
        return;
    }

    // Remove life
    PrintLine(TEXT("Lost a life!"));
    --Lives;

    // Check if lives > 0
    if(Lives < 1)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left. You have lost!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    else
    {
        // Show lives left
        FBullCowCount Score = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);
        PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
    }  
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 i=0; i < Word.Len()-1; i++)
    {
        for(int32 j=i+1; j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if(HiddenWord[i] == Guess[i])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
            
        }
    }
    return Count;
}