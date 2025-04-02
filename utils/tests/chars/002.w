
main():
	chars planet = "Mars"
	if strcmp(planet, "Earth") == 0:
		putstr("🌍 Welcome home, astronaut!")
	elif strcmp(planet, "Mars") == 0:
		putstr("👽 Greetings Martian explorer! Temperature: -80°C")
	elif strcmp(planet, "Jupiter") == 0:
		putstr("⚡ Danger! Extreme gas giant storms detected!")
	else:
		putstr("🚀 Unknown planet. Proceeding with caution...")
