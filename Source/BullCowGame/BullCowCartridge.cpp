// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

//code will allow for "simplier" Word Files
//#include "Misc/FileHelper.h"
//#include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    
    Super::BeginPlay();
    
   

    //Code will allow for "simplier" word files 
    /* const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordLists.txt");
    FFileHelper::LoadFileToStringArray(Words,*WordListPath);
    */
    Isograms = GetValidWords(Words);
    SetupGame(); //Setting Up Game
    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    
    // if game is over then do ClearScreen() and SetupGame() the game 
     if (bGameOver)      
    {
        ClearScreen();
        SetupGame();
    }
    else //Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
        
    } 
    
}

void UBullCowCartridge::SetupGame()
{
    //Welcoming the Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    
    //Game Details Setup
    HiddenWord = Isograms[FMath::RandRange(0, Isograms -1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len()); 
    PrintLine(TEXT("You have %1 lives.", Lives));
    PrintLine(TEXT("Type in your guess and Press Enter to continue..")); //Prompt Player for Guess
    PrintLIne(TEXT("The HiddenWord is: %s."), *HiddenWord); //Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("Press Enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
        {
            PrintLine(TEXT("You Have Won!"));
            EndGame();
            return;
        } 

        if(Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
            PrintLine(TEXT("Sorry, try guessing again. \nYou have %i lives remaining."),Lives);
            return;
        }
        
        //Check If Isogram
        if(!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeating letters, guess again."));
            return;
        }

        //Remove Life
        PrintLine(TEXT("You lost a Life!"));
        --Lives;

        if (Lives <= 0)
        {
            ClearScreen();
            PrintLine(TEXT("You have no lives left!"));
            PrintLine(TEXT("The hidden word was: %s"),*HiddenWord);
            EndGame();
            return;
        }
       
        //Show the player Bulls and Cows
        FBullCowCount Score = GetBullCows(Guess);

        PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score. Cows);
        PrintLine(TEXT("Guess again, you have %i lives left"), Lives);

}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
        
    }
    
    
    return true;

    // For each Letter
    // Start at element[0]
    // COmpare against the next letter
    // Until we reach [Guess.Len() -1]
    // if any are the same return false

}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
    {
        if(Word.Len() <= 8 && Word.Len() >= 4)
        {
            if(IsIsogram(Word))
            {
            
                ValidWords.Emplace(Word);
            }
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
 
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HidenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            } 
        }
    }
    return Count;
}