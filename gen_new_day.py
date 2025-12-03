import os, datetime
spaces = 4
today = datetime.date.today()

if today.month == 12:
    for i in range(1,today.day+1):
        if not os.path.exists(f"Day {i:02}"):
            os.mkdir(f"Day {i:02}")
            file = open(f"Day {i:02}/day{i:02}.cpp","w")
            template = open(f"day.template")

            file.writelines(template.readlines())
            file.close()

            file = open(f"Day {i:02}/day{i:02}.txt","w")
            file.close()

            print(f"Day {i:02} Generated")
            exit(0)
    print("All folders up to date")