extends Node

# This example shows how to use Performance Studio integration for Godot to
# add custom markers, channels, counters and Custom Acitivity Maps which can
# be viewed in Streamline. Using these features can help you profile your
# application and understand what's going on.

var frame : int = 0
var performance_studio : PerformanceStudio
var channel : PerformanceStudio_Channel
var counter : PerformanceStudio_Counter
var game_cam : PerformanceStudio_CAM
var ai_track : PerformanceStudio_CAMTrack
var ai_job : PerformanceStudio_CAMJob

# Called when the node enters the scene tree for the first time.
func _ready():
	# The Performance Studio functionality is contained in the PerformanceStudio
	# class. All other classes are created through it.
	performance_studio = PerformanceStudio.new()
	
	# Channels are custom event timelines associated with a thread. After a
	# channel has been created, you can place annotations within it.
	channel = performance_studio.create_channel("AI")
	
	# Counters are numerical data points that can be plotted as a chart in the
	# Streamline timeline view.
	# In this case, we create a counter to show how many enemies are active.
	counter = performance_studio.create_counter("Enemies", "Enemies", false)
	
	# Custom Activity Maps (CAMs) are a global (not per-thread) set of
	# timelines. Each CAM appears as its own view in the lower half of
	# Streamline's UI, so each CAM has a name, and consists of several tracks,
	# each of which appears as a named row in the CAM. Activity is logged into a
	# track by registering jobs into it.
	game_cam = performance_studio.create_cam("Game Activity")
	ai_track = game_cam.create_track("AI activity")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	frame += 1
	
	# Set the custom counter value.
	counter.set_value(frame)
	
	if frame == 50:
		# This will emit a timestamped marker with the label "Start", which
		# Streamline will show along the top of the timeline.
		performance_studio.marker("Start")
	
	if frame == 75:
		# Create an annotation. They are similar to markers, but span a range
		# of time. Creating an annotation will end any previous annotation on
		# that channel.
		channel.annotate("NPC_AI")
		
		# Create a new job on the CAMTrack and give it an associated color.
		ai_job = ai_track.create_job("NPC AI", Color8(0, 128, 128))
	
	if frame == 100:
		# Markers can be given a custom color to make them easier to identify.
		performance_studio.marker_color("Red", Color8(255, 0, 0))
	
	if frame == 465:
		# Annotations can be explicitly ended (without starting a new one).
		channel.end()
		
		# Signal the completion of the CAMJob.
		ai_job.stop()
	
	if frame == 500:
		performance_studio.marker("End")
