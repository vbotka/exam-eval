# exam-eval
Exam generates questionaire with random order of questions and answers for multiple-choice exam and Eval evaluates the results.

# exam 
exam InFile SerialNumber NoOfQuestions1 NoOfQuestions2 ...
InFile ............ file with questions and answers
SerialNumber ...... name of the output files (*.ans, *.qst)
NoOfQuestions1 .... number of questions from Chapter 1
NoOfQuestions2 .... number of questions from Chapter 2
....

# eval
eval SerialNumber


Create questionaire
# exam Questions-Answers-OS-example-SK.txt 201 3 4 3 2 2 11
Files 201.ans and 201.qst will be created.
201.qst contains 3 randomly selected questions from chapter 1, 4 randomly selected questions from chapter 2, ...
201.ans contains correct answers

Evaluate results
prepare file 201.exm with the answers and run the eval programm
# eval 201
results will be printed
